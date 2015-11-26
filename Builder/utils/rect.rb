class Rect
  attr_accessor :x, :y, :width, :height

  def initialize(x = 0, y = 0, width = 0, height = 0)
    @x = x
    @y = y
    @width = width
    @height = height
  end

  def to_s
    "#{@x} #{@y} #{@width} #{@height}"
  end
end