class Method
  def call(*args)
    @recv.__send__(@name, *args)
  end
  alias_method :[], :call

  def to_proc
    lambda { |*args|
      self.call(*args)
    }
  end

  def owner
    @owner
  end

  def receiver
    @recv
  end

  def name
    @name
  end

  def arity
    @proc.arity
  end

  def source_location
    @proc.source_location
  end

  def parameters
    @proc.parameters
  end

  def to_s
    s = "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end

class UnboundMethod
  def owner
    @owner
  end

  def name
    @name
  end

  def source_location
    @proc.source_location
  end

  def parameters
    @proc.parameters
  end

  def to_s
    s = "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end
