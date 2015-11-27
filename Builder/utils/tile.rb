class Tile

  attr_reader :pixels, :height, :width

  def initialize(pixels, width, height)
    @pixels = pixels
    @width = width
    @height = height
  end

  def == (other)
    @width == other.width && @height == other.height && @pixels == other.pixels
  end

end