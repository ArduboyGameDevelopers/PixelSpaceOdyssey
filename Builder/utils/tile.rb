require 'chunky_png'

class Tile

  attr_reader :pixels, :height, :width, :name

  def initialize(pixels, width, height, name = nil)
    @pixels = pixels
    @width = width
    @height = height
    @name = name
  end

  def == (other)
    @width == other.width && @height == other.height && @pixels == other.pixels
  end

  def to_png
    ChunkyPNG::Image.new @pixels, @width, @height
  end

end