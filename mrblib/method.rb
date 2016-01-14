class Method
  def ==(other)
    return false unless Method === other
    return false if self.class != other.class
    return false if owner != other.owner
    return false if @recv != other.instance_variable_get(:@recv)

    true
  end

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
    "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end
