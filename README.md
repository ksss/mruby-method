mruby-method
===

[![Build Status](https://travis-ci.org/ksss/mruby-method.svg)](https://travis-ci.org/ksss/mruby-method)

A implementetion of class **Method** and **UnboundMethod** for mruby

```ruby
p Enumerable.instance_method(:find_all).source_location
#=> ["mruby/mruby/mrblib/enum.rb", 148]
```

![https://raw.githubusercontent.com/ksss/mruby-method/master/find_all.png](https://raw.githubusercontent.com/ksss/mruby-method/master/find_all.png)

# Note

`source_location` method need this configuration in build_config.rb

```
MRuby::Build.new do |conf|
  enable_debug
end
```
