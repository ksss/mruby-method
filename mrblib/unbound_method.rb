class UnboundMethod
  def owner
    @owner
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
