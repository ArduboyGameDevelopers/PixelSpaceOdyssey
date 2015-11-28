require 'psd'

class TileSet

  TILE_WIDTH = 8
  TILE_HEIGHT = 8

  attr_reader :name, :tiles

  def self.from_psd(psd_file)
    name = Utils.to_identifier File.basename(psd_file, '.psd')
    tile_set = TileSet.new name

    PSD.open(psd_file) do |psd|
      children = psd.tree.children
      children.each { |child|
        tiles = tiles_from_group(child) if child.is_a? PSD::Node::Group
        tiles.each { |tile|
          tile_set.add_tile tile
        }
      }
    end

    tile_set
  end

  def initialize(name)
    @name = name
    @tiles = []
  end

  def add_tile(tile)
    raise 'Duplicate tile' if @tiles.include? tile
    @tiles << tile
  end

  def index(tile)
    @tiles.index tile
  end

  private
  def self.tiles_from_group(group)
    tiles = []

    group.children.each { |child|
      next unless child.is_a? PSD::Node::Layer
      layer_tiles = tiles_from_layer(child)
      layer_tiles.each { |tile|
        raise "Duplicate tile: '#{group.name}/#{child.name}'" if tiles.include? tile
        tiles << tile
      }
    }

    tiles
  end

  private
  def self.tiles_from_layer(layer)
    layer_png = layer.image.to_png
    width = layer_png.width
    height = layer_png.height

    parent_name = layer.parent.name

    raise "Layer '#{parent_name}/#{layer.name}' has illegal width: #{width}" if (width % TILE_WIDTH) != 0
    raise "Layer '#{parent_name}/#{layer.name}' has illegal height: #{height}" if (height % TILE_HEIGHT) != 0

    rows = height / TILE_HEIGHT
    cols = width / TILE_WIDTH

    basename = Utils.to_identifier "#{parent_name}"
    basename << Utils.to_identifier("_#{layer.name}") if layer.parent.children.count > 1

    tiles = []
    (0..rows-1).each { |i|
      (0..cols-1).each { |j|
        index = i * cols + j
        tile_image = layer_png.sub_image j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT
        tile_name = rows > 1 || cols > 1 ? "#{basename}_#{index + 1}" : basename
        tile = Tile.new tile_image.pixels, TILE_WIDTH, TILE_HEIGHT, tile_name
        raise "Duplicate tile in layer '#{layer.name}'" if tiles.include? tile
        tiles << tile
      }
    }
    tiles
  end

end