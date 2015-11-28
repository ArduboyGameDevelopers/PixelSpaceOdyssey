class TileMap

  attr_reader :height, :indices, :width

  def initialize(indices, width, height)
    @indices = indices
    @width = width
    @height = height
  end

end