#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"
#include "mruby/proc.h"

static struct RObject *
method_object_alloc(
  mrb_state *mrb,
  struct RClass *rclass,
  struct RClass *owner,
  mrb_value recv,
  mrb_sym name,
  struct RProc *proc
) {
  struct RClass *method = mrb_class_get(mrb, "Method");
  struct RObject *me = (struct RObject*)mrb_obj_alloc(mrb, MRB_TT_CLASS, method);

  mrb_obj_iv_set(mrb, me, mrb_intern_lit(mrb, "@rclass"), mrb_obj_value(rclass));
  mrb_obj_iv_set(mrb, me, mrb_intern_lit(mrb, "@owner"), mrb_obj_value(owner));
  mrb_obj_iv_set(mrb, me, mrb_intern_lit(mrb, "@recv"), recv);
  mrb_obj_iv_set(mrb, me, mrb_intern_lit(mrb, "@name"), mrb_symbol_value(name));
  mrb_obj_iv_set(mrb, me, mrb_intern_lit(mrb, "@proc"), mrb_obj_value(proc));

  return me;
}

static struct RObject *
unbound_method_object_alloc(
  mrb_state *mrb,
  struct RClass *rclass,
  struct RClass *owner,
  mrb_sym name,
  struct RProc *proc
) {
  struct RClass *unbound_method = mrb_class_get(mrb, "UnboundMethod");
  struct RObject *ume = (struct RObject*)mrb_obj_alloc(mrb, MRB_TT_CLASS, unbound_method);

  mrb_obj_iv_set(mrb, ume, mrb_intern_lit(mrb, "@rclass"), mrb_obj_value(rclass));
  mrb_obj_iv_set(mrb, ume, mrb_intern_lit(mrb, "@owner"), mrb_obj_value(owner));
  mrb_obj_iv_set(mrb, ume, mrb_intern_lit(mrb, "@name"), mrb_symbol_value(name));
  mrb_obj_iv_set(mrb, ume, mrb_intern_lit(mrb, "@proc"), mrb_obj_value(proc));

  return ume;
}

static mrb_value
unbound_method_bind(mrb_state *mrb, mrb_value self)
{
  struct RObject *me;
  mrb_value rclass = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@rclass"));
  mrb_value owner = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@owner"));
  mrb_value name = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@name"));
  mrb_value proc = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@proc"));
  mrb_value recv;

  mrb_get_args(mrb, "o", &recv);

  if (!mrb_obj_is_kind_of(mrb, recv, mrb_class_ptr(owner))) {
    mrb_raisef(mrb, E_TYPE_ERROR, "bind argument must be an instance of %S", owner);
  }
  me = method_object_alloc(mrb,
    mrb_class_ptr(rclass),
    mrb_class_ptr(owner),
    recv,
    mrb_symbol(name),
    mrb_proc_ptr(proc)
  );
  return mrb_obj_value(me);
}

static mrb_value
method_unbind(mrb_state *mrb, mrb_value self)
{
  struct RObject *ume;
  mrb_value rclass = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@rclass"));
  mrb_value owner = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@owner"));
  mrb_value name = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@name"));
  mrb_value proc = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@proc"));
  ume = unbound_method_object_alloc(mrb,
    mrb_class_ptr(rclass),
    mrb_class_ptr(owner),
    mrb_symbol(name),
    mrb_proc_ptr(proc)
  );
  return mrb_obj_value(ume);
}

static void
mrb_search_method_owner(mrb_state *mrb, struct RClass *c, mrb_value obj, mrb_sym name, struct RClass **owner, struct RProc **proc)
{
  *owner = c;
  *proc = mrb_method_search_vm(mrb, owner, name);
  if (!*proc) {
    mrb_sym respond_to_missing = mrb_intern_lit(mrb, "respond_to_missing?");
    mrb_value str_name = mrb_sym2str(mrb, name);
    *owner = c;
    *proc = mrb_method_search_vm(mrb, owner, respond_to_missing);
    if (*proc) {
      if (mrb_test(mrb_funcall(mrb, obj, "respond_to_missing?", 2, mrb_symbol_value(name), mrb_false_value()))) {
        *owner = c;
      }
      else {
        const char *s = mrb_class_name(mrb, c);
        mrb_raisef(mrb, E_NAME_ERROR, "undefined method `%S' for class `%S'", str_name, mrb_str_new_static(mrb, s, strlen(s)));
      }
    }
    else {
      const char *s = mrb_class_name(mrb, c);
      mrb_raisef(mrb, E_NAME_ERROR, "undefined method `%S' for class `%S'", str_name, mrb_str_new_static(mrb, s, strlen(s)));
    }
  }
}

static mrb_value
mrb_kernel_method(mrb_state *mrb, mrb_value self)
{
  struct RClass *owner;
  struct RProc *proc;
  struct RObject *me;
  mrb_sym name;

  mrb_get_args(mrb, "n", &name);

  mrb_search_method_owner(mrb, mrb_class(mrb, self), self, name, &owner, &proc);

  me = method_object_alloc(mrb, mrb_obj_class(mrb, self), owner, self, name, proc);
  return mrb_obj_value(me);
}

static mrb_value
mrb_module_instance_method(mrb_state *mrb, mrb_value self)
{
  struct RClass *owner;
  struct RProc *proc;
  struct RObject *ume;
  mrb_sym name;

  mrb_get_args(mrb, "n", &name);

  mrb_search_method_owner(mrb, mrb_class_ptr(self), self, name, &owner, &proc);

  ume = unbound_method_object_alloc(mrb, mrb_class_ptr(self), owner, name, proc);

  return mrb_obj_value(ume);
}

void
mrb_mruby_method_gem_init(mrb_state* mrb)
{
  struct RClass *unbound_method = mrb_define_class(mrb, "UnboundMethod", mrb->object_class);
  struct RClass *method = mrb_define_class(mrb, "Method", mrb->object_class);

  mrb_undef_class_method(mrb, unbound_method, "new");
  mrb_define_method(mrb, unbound_method, "bind", unbound_method_bind, MRB_ARGS_REQ(1));

  mrb_undef_class_method(mrb, method, "new");
  mrb_define_method(mrb, method, "unbind", method_unbind, MRB_ARGS_NONE());

  mrb_define_method(mrb, mrb->kernel_module, "method", mrb_kernel_method, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, mrb->module_class, "instance_method", mrb_module_instance_method, MRB_ARGS_REQ(1));
}

void
mrb_mruby_method_gem_final(mrb_state* mrb)
{
}
