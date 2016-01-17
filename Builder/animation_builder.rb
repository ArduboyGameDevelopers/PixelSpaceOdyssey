require 'psd'
require_relative 'common'

Dir['utils/**/*.rb'].each { |source_file| require_relative source_file }

class AnimationBuilder

  TRANSPARENT_COLOR = 0xffffffff

  def self.generate(psd_file, output_dir)
    PSD.open(psd_file) do |psd|
      animation_name = get_animation_name psd_file
      animation_set = AnimationSet.new animation_name, psd.image.width, psd.image.height

      process_guides animation_set, psd

      children = psd.tree.children
      children.each { |child|
        animation_set.add_animation process_group(child) if child.is_a? PSD::Node::Group
      }

      write_animation_set animation_set, output_dir
    end
  end

  private
  def self.process_guides(animation_set, psd)

    resources = psd.resources
    guides_resource = resources[1032]
    return if guides_resource.nil?

    vertical = []
    horizontal = []
    guides_resource.data.to_a.each do |guide|
      location = guide[:location]
      if guide[:direction] == 'vertical'
        vertical.push location
      else
        horizontal.push location
      end
    end

    raise "Wrong number of horizontal guides: #{horizontal.length}" if horizontal.length > 3
    raise "Wrong number of vertical guides: #{vertical.length}" if vertical.length > 3

    vertical.sort!
    horizontal.sort!

    center = animation_set.center
    bounds = animation_set.bounds

    if vertical.length != 0
      if vertical.length == 1
        center.x = vertical[0]
      elsif vertical.length == 2
        min = vertical[0]
        max = vertical[1]
        bounds.width = max - min
        bounds.x = min
        center.x = min + bounds.width / 2
      elsif vertical.length == 3
        min = vertical[0]
        max = vertical[2]
        bounds.width = max - min
        bounds.x = min
        center.x = vertical[1]
      end
    end

    if horizontal.length != 0
      if horizontal.length == 1
        min = horizontal[0]
        max = animation_set.height
        bounds.height = max - min
        bounds.y = min
        center.y = min + bounds.height / 2
      else
        raise "Not supported number of horizontal guides: #{horizontal.length}"
      end
    end

    animation_set.center = center
    animation_set.bounds = bounds
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
  def self.write_animation_set(animation_set, output_dir)
    basename = animation_set.name
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
    animation_initializers = []

    total_frames = 0

    animations = animation_set.animations
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

      animation_initializer = "AnimationMake(#{frames_name}, #{frames.length}, #{animation_set.width}, #{animation_set.height}"
      animation_initializer << ", #{animation.looped}" if animation.looped
      animation_initializer << ')'
      animation_initializers << animation_initializer
    }

    puts total_frames

    source_h.println
    source_h.println "#define #{basename.upcase}_ANIMATIONS_COUNT #{animation_initializers.length}"

    source_h.println
    source_h.println "extern const Animation #{basename.upcase}_ANIMATIONS[];"

    source_h.println
    source_h.println "#define #{basename.upcase}_WIDTH #{animation_set.bounds.width}"
    source_h.println "#define #{basename.upcase}_HEIGHT #{animation_set.bounds.height}"

    source_cpp.println
    source_cpp.println "const Animation #{basename.upcase}_ANIMATIONS[] = "
    source_cpp.block_open
    animation_initializers.each { |a| source_cpp.println "#{a}," }
    source_cpp.block_close ';'

    source_h.println
    source_h.println "#endif /* #{define_name} */"

    source_h.write_to_file file_h
    source_cpp.write_to_file file_cpp
  end

  private
  def self.get_animation_name(file)
    basename = File.basename(file, '.psd')
    at_index = basename.index '@'
    basename = basename[0..at_index-1] if at_index != nil
    Utils.to_identifier basename
  end

end