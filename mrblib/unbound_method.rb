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
    "#<#{self.class}: #{@owner}##{@name}>"
  end
  alias inspect to_s
end
