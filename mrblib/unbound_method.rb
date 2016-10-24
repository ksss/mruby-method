class UnboundMethod
  def ==(other)
    return false unless UnboundMethod === other
    return false if self.class != other.class
    return false if owner != other.owner
    @proc == other.instance_variable_get(:@proc)
  end
  alias_method :eql?, :==

  def owner
    @owner
  end

  def name
    @name
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
