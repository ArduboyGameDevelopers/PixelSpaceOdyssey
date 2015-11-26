require_relative 'rect'

class Frame
  attr_reader :name, :data, :rect

  def initialize(name, data, rect)
    @name = name
    @data = data
    @rect = rect
  end

  def x
    @rect.x
  end

  def y
    @rect.y
  end

  def width
    @rect.width
  end

  def height
    @rect.height
  end

  def to_s
    "#{name}: #{rect.width}x#{rect.height}"
  end

  def to_pattern
    result = ''
    (0..height-1).each do |y|
      (0..width-1).each do |x|
        bit = @data[y * width + x]
        result << (bit == 1 ? '·' : ' ')
      end
      result = result + "\n" unless y == height - 1
    end
    result
  end

end