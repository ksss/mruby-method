class Method
  def ==(other)
    return false unless other.instance_of?(Method)
    return false if self.class != other.class
    return false if owner != other.owner
    return false if @recv != other.receiver
    @proc == other.instance_variable_get(:@proc)
  end
  alias_method :eql?, :==

  def to_proc
    m = self
    lambda { |*args|
      m.call(*args)
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
    if @proc
      @proc.arity
    else
      -1
    end
  end

  def source_location
    if @proc
      @proc.source_location
    else
      nil
    end
  end

  def parameters
    if @proc
      @proc.parameters
    else
      [[:rest]]
    end
  end

  def to_s
    "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end
