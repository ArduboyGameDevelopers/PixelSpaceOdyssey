require 'psd'
require_relative 'common'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

include Common

class TileMapBuilder

  TILE_WIDTH  = 8
  TILE_HEIGHT = 8

  def self.generate(psd_file, tile_set, output_dir)
    PSD.open(psd_file) do |psd|
      tilemap_layer = find_tilemap psd
      fail_script_unless tilemap_layer != nil, "Can't find 'Tilemap' layer: #{psd_file}"

      tilemap_png = tilemap_layer.image.to_png

      layer_png = ChunkyPNG::Image.new psd.width, psd.height
      layer_png.compose! tilemap_png, tilemap_layer.left, tilemap_layer.top

      name = File.basename(psd_file, '.psd').downcase
      generate_from_png name, layer_png, tile_set, output_dir
    end
  end

  private
  def self.generate_from_png(name, png, tile_set, output_dir)

    width = png.width
    height = png.height

    raise "Illegal image width: #{width}" if (width % TILE_WIDTH) != 0
    raise "Illegal image height: #{height}" if (height % TILE_HEIGHT) != 0

    rows = height / TILE_HEIGHT
    cols = width / TILE_WIDTH

    indices = []
    unknown = []
    (0..rows-1).each do |row|
      y = TILE_HEIGHT * row
      (0..cols-1).each do |col|
        x = TILE_WIDTH * col
        tile = png.sub_image x, y, TILE_WIDTH, TILE_HEIGHT
        if tile.is_transparent
          indices.push 0
          next
        end

        index = tile_set.index tile
        if index.nil?
          patch = ChunkyPNG::Image.new TILE_WIDTH, TILE_HEIGHT, 0xff00ff7f
          png.compose! patch, x, y
          unknown << tile
          next
        end

        indices.push index + 1
      end
    end

    if unknown.length > 0
      png_file = File.expand_path 'unknown.png'
      png.save png_file
      raise "Unknown tile: #{png_file}"
    end

    tile_map = TileMap.new indices, cols, rows
    write_tile_map name, tile_map, output_dir
  end

  private
  def self.write_tile_map(name, tile_map, output_dir)

    source_h = SourceWriter.new
    source_cpp = SourceWriter.new

    basename = Utils.to_identifier name
    filename = "tiles_#{basename}"
    defines_name = "#{filename}_h"
    indices_var_name = "INDICES_#{basename.upcase}"

    source_h.println "#ifndef #{defines_name}"
    source_h.println "#define #{defines_name}"
    source_h.println
    source_h.println "#include \"common.h\""
    source_h.println
    source_h.println "#define TILEMAP_#{basename.upcase}_WIDTH  #{tile_map.width}"
    source_h.println "#define TILEMAP_#{basename.upcase}_HEIGHT #{tile_map.height}"
    source_h.println
    source_h.println "extern const uint8_t #{indices_var_name}[];"

    source_cpp.println '#include <avr/pgmspace.h>'
    source_cpp.println

    source_cpp.println "#include \"#{filename}.h\""
    source_cpp.println "#include \"common.h\""
    source_cpp.println

    source_cpp.println "PROGMEM const uint8_t #{indices_var_name}[] ="
    source_cpp.block_open
    (0..tile_map.height-1).each do |r|
      (0..tile_map.width-1).each do |c|
        index = r * tile_map.width + c
        source_cpp.print "#{tile_map.indices[index]},"
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