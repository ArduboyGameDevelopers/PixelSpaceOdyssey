class Tilemap

  attr_reader :height, :indices, :width, :tiles

  def initialize(tiles, indices, width, height)
    @tiles = tiles
    @indices = indices
    @width = width
    @height = height
  end

end