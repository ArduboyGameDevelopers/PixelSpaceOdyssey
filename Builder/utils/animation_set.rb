require_relative 'rect'

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

class AnimationSet

  attr_reader :height, :name, :width, :animations
  attr_accessor :center, :bounds

  def initialize(name, width, height)
    @name = name
    @width = width
    @height = height
    @center = Point.new(width / 2, height / 2)
    @bounds = Rect.new(0, 0, width, height)
    @animations = []
  end

  def add_animation(animation)
    @animations.push animation
  end

end