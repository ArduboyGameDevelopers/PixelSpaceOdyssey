class Animation

  attr_reader :name, :frames

  def initialize(name)
    @name = name
    @frames = []
  end

  def add_frame(frame)
    @frames.unshift frame
  end

end