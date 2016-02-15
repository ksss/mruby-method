class UnboundMethod
  def ==(other)
    return false unless UnboundMethod === other
    return false if self.class != other.class
    return false if owner != other.owner
    return false if @name != other.name

    true
  end
  alias_method :eql?, :==

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
    "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end
