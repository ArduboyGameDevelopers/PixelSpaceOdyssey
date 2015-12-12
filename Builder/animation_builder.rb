require 'psd'
require_relative 'common'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

class AnimationBuilder

  TRANSPARENT_COLOR = 0xffffffff

  def self.generate(psd_file, output_dir)

    animations = []

    PSD.open(psd_file) do |psd|
      children = psd.tree.children
      children.each { |child|
        animations.push process_group(child) if child.is_a? PSD::Node::Group
      }
    end

    write_animations psd_file, animations, output_dir
  end

  private
  def self.process_group(group)
    puts group.name

    animation = Animation.new group.name
    group.children.each { |child|
      next unless child.is_a? PSD::Node::Layer
      animation.add_frame process_layer(child)
    }

    animation
  end

  private
  def self.process_layer(layer)
    ImageUtils::make_frame(layer.name, layer.image.to_png, TRANSPARENT_COLOR)
  end

  private
  def self.write_animations(psd_file, animations, output_dir)
    basename = File.basename(psd_file, '.psd').downcase
    filename = "#{basename}_animations"

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

    animation_defines = []
    animation_inializers = []

    total_frames = 0;

    (0..animations.length-1).each { |animation_index|

      animation = animations[animation_index]
      animation_name = Utils.to_identifier animation.name

      frames = animation.frames
      frames_names = []
      total_frames += frames.count

      (0..frames.count-1).each { |index|

        frame = frames[index]

        data = []
        frame.data.each { |byte|
          data << "0x#{byte.to_s 16}"
        }

        var_name = "FRAME_#{Utils.to_identifier("#{animation_name}_#{index}").upcase}"
        frames_names << var_name

        source_cpp.println
        source_cpp.println "static PROGMEM WEAK_CONST unsigned char #{var_name}[] ="
        source_cpp.block_open
        source_cpp.println "#{frame.x}, #{frame.y}, #{frame.width}, #{frame.height},"
        source_cpp.println "#{data.join ', '}"
        source_cpp.block_close ';'
      }

      frames_name = "FRAMES_#{Utils.to_identifier(animation_name).upcase}"

      source_cpp.println
      source_cpp.println "static PgmPtr #{frames_name}[] ="

      source_cpp.block_open
      frames_names.each do |name|
        source_cpp.println "#{name},"
      end
      source_cpp.block_close ';'

      animation_define = "#{basename.upcase}_ANIMATION_#{animation_name.upcase}"
      raise "Duplication animation: #{animation_name}" if animation_defines.include? animation_define
      animation_defines << animation_define

      source_h.println
      source_h.println "#define #{animation_define} #{animation_index}"

      animation_inializers << "AnimationMake(#{frames_name}, #{frames.length})"
    }

    puts total_frames

    source_h.println
    source_h.println "#define #{basename.upcase}_ANIMATIONS_COUNT #{animation_inializers.length}"

    source_h.println
    source_h.println "extern const Animation #{basename.upcase}_ANIMATIONS[];"

    source_cpp.println
    source_cpp.println "const Animation #{basename.upcase}_ANIMATIONS[] = "
    source_cpp.block_open
    animation_inializers.each { |a| source_cpp.println "#{a}," }
    source_cpp.block_close ';'

    source_h.println
    source_h.println "#endif /* #{define_name} */"

    source_h.write_to_file file_h
    source_cpp.write_to_file file_cpp
  end

end