class Method
  def to_proc
    m = self
    lambda { |*args, &b|
      m.call(*args, &b)
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
end
