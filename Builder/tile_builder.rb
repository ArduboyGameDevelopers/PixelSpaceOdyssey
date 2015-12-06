require 'psd'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

class TileBuilder

  def self.generate(psd_file, output_dir)

    tile_set = TileSet.from_psd psd_file
    write_tile_set tile_set, output_dir

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

  private
  def self.write_tile_set(tile_set, output_dir)
    source_h = SourceWriter.new
    source_cpp = SourceWriter.new

    basename = Utils.to_identifier tile_set.name
    filename = "tiles_#{basename}"
    defines_name = "#{filename}_h"
    tiles_var_name   = "TILES_#{basename.upcase}"

    tiles = tile_set.tiles

    source_h.println "#ifndef #{defines_name}"
    source_h.println "#define #{defines_name}"
    source_h.println
    source_h.println "#include \"common.h\""
    source_h.println
    source_h.println "#define #{tiles_var_name}_COUNT #{tiles.count}"
    source_h.println
    source_h.println "extern PgmPtr WEAK_CONST #{tiles_var_name}[];"

    source_cpp.println '#include <avr/pgmspace.h>'
    source_cpp.println

    source_cpp.println "#include \"#{filename}.h\""
    source_cpp.println "#include \"common.h\""
    source_cpp.println

    tiles.each do |tile|
      prog_mem = tile.to_prog_mem
      source_cpp.println "PROGMEM static WEAK_CONST unsigned char #{tiles_var_name}_#{Utils.to_identifier(tile.name).upcase}[] = "
      source_cpp.block_open
      source_cpp.println prog_mem.to_code
      source_cpp.block_close ';'
      source_cpp.println
    end

    source_cpp.println "PgmPtr WEAK_CONST #{tiles_var_name}[] ="
    source_cpp.block_open
    tiles.each_with_index do |tile, index|
      source_cpp.print "#{tiles_var_name}_#{Utils.to_identifier(tile.name).upcase} /* #{index + 1} */"
      source_cpp.println (index < tiles.length-1 ? ',' : '')
    end
    source_cpp.block_close ';'
    source_cpp.println

    source_h.println
    source_h.println "#endif /* #{defines_name} */"

    source_h.write_to_file "#{output_dir}/#{filename}.h"
    source_cpp.write_to_file "#{output_dir}/#{filename}.cpp"
  end

end