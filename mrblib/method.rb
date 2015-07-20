class Method
  def initialize(rclass, owner, recv, name, pr)
    @rclass = rclass
    @owner = owner
    @recv = recv
    @name = name
    @proc = pr
  end

  def call(*args)
    @recv.__send__(@name, *args)
  end

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

  def unbind
    UnboundMethod.new(@rclass, @owner, @name, @proc)
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
  def initialize(rclass, owner, name, pr)
    @rclass = rclass
    @owner = owner
    @name = name
    @proc = pr
  end

  def bind(recv)
    unless recv.kind_of?(@owner)
      raise TypeError, "bind argument must be an instance of #{@owner}"
    end
    Method.new(@rclass, @owner, recv, @name, @proc)
  end

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
