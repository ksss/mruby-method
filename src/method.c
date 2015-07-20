#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"

static void
mrb_search_method_owner(mrb_state *mrb, struct RClass *c, mrb_value obj, mrb_value name, struct RClass **owner, struct RProc **proc)
{
  *owner = c;
  *proc = mrb_method_search_vm(mrb, owner, mrb_symbol(name));
  if (!*proc) {
    mrb_sym respond_to_missing = mrb_intern_lit(mrb, "respond_to_missing?");
    *owner = c;
    *proc = mrb_method_search_vm(mrb, owner, respond_to_missing);
    if (*proc) {
      if (mrb_test(mrb_funcall(mrb, obj, "respond_to_missing?", 2, name, mrb_false_value()))) {
        *owner = c;
      }
      else {
        mrb_raisef(mrb, E_ARGUMENT_ERROR, "no search method `%S'", name);
      }
    }
    else {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "no search method `%S'", name);
    }
  }
}

static mrb_value
mrb_kernel_method(mrb_state *mrb, mrb_value self)
{
  struct RClass *method = mrb_class_get(mrb, "Method");
  struct RClass *owner;
  struct RProc *proc;
  mrb_value name;
  mrb_value me;
  mrb_value argv[5];

  mrb_get_args(mrb, "o", &name);

  mrb_search_method_owner(mrb, mrb_class(mrb, self), self, name, &owner, &proc);

  argv[0] = mrb_obj_value(mrb_obj_class(mrb, self));
  argv[1] = mrb_obj_value(owner);
  argv[2] = self;
  argv[3] = name;
  argv[4] = mrb_obj_value(proc);
  me = mrb_obj_new(mrb, method, 5, argv);

  return me;
}

static mrb_value
mrb_module_instance_method(mrb_state *mrb, mrb_value self)
{
  struct RClass *unbound_method = mrb_class_get(mrb, "UnboundMethod");
  struct RClass *owner;
  struct RProc *proc;
  mrb_value name;
  mrb_value ume;
  mrb_value argv[4];

  mrb_get_args(mrb, "o", &name);

  mrb_search_method_owner(mrb, mrb_class_ptr(self), self, name, &owner, &proc);

  argv[0] = self;
  argv[1] = mrb_obj_value(owner);
  argv[2] = name;
  argv[3] = mrb_obj_value(proc);
  ume = mrb_obj_new(mrb, unbound_method, 4, argv);

  return ume;
}

void
mrb_mruby_method_gem_init(mrb_state* mrb)
{
  mrb_define_class(mrb, "UnboundMethod", mrb->object_class);
  mrb_define_class(mrb, "Method", mrb->object_class);

  mrb_define_method(mrb, mrb->kernel_module, "method", mrb_kernel_method, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, mrb->module_class, "instance_method", mrb_module_instance_method, MRB_ARGS_REQ(1));
}

void
mrb_mruby_method_gem_final(mrb_state* mrb)
{
}
