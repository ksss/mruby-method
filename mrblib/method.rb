class Method
  def initialize(rclass, owner, recv, id, pr)
    @rclass = rclass
    @owner = owner
    @recv = recv
    @id = id
    @proc = pr
  end

  def call(*args)
    @recv.__send__(@id, *args)
  end

  def owner
    @owner
  end

  def receiver
    @recv
  end

  def name
    @id
  end

  def unbind
    UnboundMethod.new(@rclass, @owner, @id, @proc)
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
    s = "#<#{self.class}: #{@owner}##{@id}>"
  end
  alias inspect to_s
end

class UnboundMethod
  def initialize(rclass, owner, id, pr)
    @rclass = rclass
    @owner = owner
    @id = id
    @proc = pr
  end

  def bind(recv)
    unless recv.kind_of?(@owner)
      raise TypeError, "bind argument must be an instance of #{@owner}"
    end
    Method.new(@rclass, @owner, recv, @id, @proc)
  end

  def owner
    @owner
  end

  def name
    @id
  end

  def source_location
    @proc.source_location
  end

  def parameters
    @proc.parameters
  end

  def to_s
    s = "#<#{self.class}: #{@owner}##{@id}>"
  end
  alias inspect to_s
end
