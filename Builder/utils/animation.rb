class Animation

  LOOPED_SUFFIX = '(looped)'

  attr_reader :name, :frames, :looped

  def initialize(name)
    @looped = name.end_with? LOOPED_SUFFIX
    @name = @looped ? name.slice(0..name.length - LOOPED_SUFFIX.length - 1).strip : name.strip
    @frames = []
  end

  def add_frame(frame)
    @frames.unshift frame
  end

end