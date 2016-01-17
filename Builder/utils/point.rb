class Point
  attr_accessor :x, :y

  def initialize(x = 0, y = 0)
    @x = x
    @y = y
  end

  def to_s
    "x:#{@x} y:#{@y}"
  end
end