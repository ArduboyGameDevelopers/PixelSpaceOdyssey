require 'psd'
require_relative 'common'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

include Common

class TileMapBuilder

  TILE_WIDTH  = 8
  TILE_HEIGHT = 8

  def self.generate(psd_file, output_dir)
    PSD.open(psd_file) do |psd|
      tilemap_layer = find_tilemap psd
      fail_script_unless tilemap_layer != nil, "Can't find 'Tilemap' layer: #{psd_file}"

      tilemap_png = tilemap_layer.image.to_png

      layer_png = ChunkyPNG::Image.new psd.width, psd.height
      layer_png.compose! tilemap_png, tilemap_layer.left, tilemap_layer.top

      name = File.basename(psd_file, '.psd').downcase
      generate_from_png name, layer_png, output_dir
    end

  end

  private
  def self.generate_from_png(name, png, output_dir)

    width = png.width
    height = png.height

    rows = height / TILE_HEIGHT
    cols = width / TILE_WIDTH

    tiles = []
    indices = []
    (0..rows-1).each do |row|
      y = TILE_HEIGHT * row
      (0..cols-1).each do |col|
        x = TILE_WIDTH * col
        tile = png.sub_image x, y, TILE_WIDTH, TILE_HEIGHT
        if tile.is_transparent
          indices.push 0
          next
        end

        index = tiles.index tile
        if index.nil?
          indices.push tiles.length + 1
          tiles.push tile
        else
          indices.push index + 1
        end
      end
    end

    tilemap = Tilemap.new tiles, indices, cols, rows
    write_tilemap name, tilemap, output_dir
  end

  private
  def self.write_tilemap(name, tilemap, output_dir)

    source_h = SourceWriter.new
    source_cpp = SourceWriter.new

    basename = Utils.to_identifier name
    filename = "tiles_#{basename}"
    defines_name = "#{filename}_h"
    tiles_var_name   = "TILES_#{basename.upcase}"
    indices_var_name = "INDICES_#{basename.upcase}"

    tiles = tilemap.tiles

    source_h.println "#ifndef #{defines_name}"
    source_h.println "#define #{defines_name}"
    source_h.println
    source_h.println "#include \"common.h\""
    source_h.println
    source_h.println "#define TILEMAP_#{basename.upcase}_WIDTH  #{tilemap.width}"
    source_h.println "#define TILEMAP_#{basename.upcase}_HEIGHT #{tilemap.height}"
    source_h.println
    source_h.println "extern PgmPtr const #{tiles_var_name}[];"
    source_h.println "extern const uint8_t #{indices_var_name}[];"

    source_cpp.println '#include <avr/pgmspace.h>'
    source_cpp.println

    source_cpp.println "#include \"#{filename}.h\""
    source_cpp.println "#include \"common.h\""
    source_cpp.println

    (0..tiles.length-1).each do |index|
      prog_mem = tiles[index].to_prog_mem
      source_cpp.println "PROGMEM static const unsigned char #{tiles_var_name}_#{index + 1}[] = "
      source_cpp.block_open
      source_cpp.println prog_mem.to_code
      source_cpp.block_close ';'
      source_cpp.println
    end

    source_cpp.println "PgmPtr const #{tiles_var_name}[] ="
    source_cpp.block_open
    (0..tiles.length-1).each do |index|
      source_cpp.print "#{tiles_var_name}_#{index + 1}"
      source_cpp.println (index < tiles.length-1 ? ',' : '')
    end
    source_cpp.block_close ';'
    source_cpp.println

    source_cpp.println "const uint8_t #{indices_var_name}[] ="
    source_cpp.block_open
    (0..tilemap.height-1).each do |r|
      (0..tilemap.width-1).each do |c|
        index = r * tilemap.width + c
        source_cpp.print "#{tilemap.indices[index]},"
      end
      source_cpp.println
    end
    source_cpp.block_close ';'

    source_h.println
    source_h.println "#endif /* #{defines_name} */"

    source_h.write_to_file "#{output_dir}/#{filename}.h"
    source_cpp.write_to_file "#{output_dir}/#{filename}.cpp"
  end

  private
  def self.find_tilemap(psd)
    children = psd.tree.children
    children.each { |child|
      return child if child.is_a? PSD::Node::Layer and child.name == 'Tilemap'
    }
    nil
  end

end