require 'psd'
require_relative 'common'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

class ImageBuilder

  TRANSPARENT_COLOR = 0xffffffff

  def self.generate(dir_images, dir_output)

    images = []

    Dir["#{dir_images}/*.png"].each do |file_image|
      images.push ImageUtils.read_image file_image
    end

    write_images images, dir_output
  end

  private
  def self.write_images(images, output_dir)

    filename = 'images'

    file_h = "#{output_dir}/#{filename}.h"
    file_cpp = "#{output_dir}/#{filename}.cpp"

    define_name = "#{Utils.to_identifier filename}_h"

    source_h = SourceWriter.new
    source_cpp = SourceWriter.new

    source_h.println "#ifndef #{define_name}"
    source_h.println "#define #{define_name}"
    source_h.println
    source_h.println "#include \"animation.h\""

    source_cpp.println '#include <avr/pgmspace.h>'
    source_cpp.println
    source_cpp.println "#include \"#{File.basename file_h}\""

    images.each do |image|

      image_name = Utils.to_identifier image.name

      data = []
      image.data.each { |byte|
        data << "0x#{byte.to_s 16}"
      }

      var_name = "IMG_#{Utils.to_identifier(image_name).upcase}"

      source_h.println
      source_h.println "extern PROGMEM WEAK_CONST unsigned char #{var_name}[];"

      source_cpp.println
      source_cpp.println "PROGMEM WEAK_CONST unsigned char #{var_name}[] ="
      source_cpp.block_open
      source_cpp.println "#{image.width}, #{image.height},"
      source_cpp.println "#{data.join ', '}"
      source_cpp.block_close ';'

    end

    source_h.println
    source_h.println "#endif /* #{define_name} */"

    source_h.write_to_file file_h
    source_cpp.write_to_file file_cpp
  end
end