require 'chunky_png'

class ProgMem

  attr_reader :height, :width, :bytes

  def self.from_png(png)
    from_pixels png.pixels, png.width, png.height
  end

  def self.from_pixels(pixels, width, height)
    bits_last_page = height % 8
    bytes_high = height / 8
    bytes_high += 1 if bits_last_page > 0

    bytes = []

    (0..bytes_high - 1).each do |y_page|
      (0..width - 1).each do |x|
        # how many bits does this line hold
        bits = 8

        # if we've reached the bottom there are fewer bits to load
        bits = bits_last_page if bytes_high - 1 == y_page and bits_last_page > 0
        byte = 0
        (0..bits-1).each do |bit_height|
          pixel_index = (y_page * 8 + bit_height) * width + x
          pixel = pixels[pixel_index]
          byte |= (1 << (bit_height)) if ChunkyPNG::Color.a(pixel) > 128
        end
        bytes << byte
      end
    end

    ProgMem.new bytes, width, height
  end

  def initialize(bytes, width, height)
    @bytes  = bytes
    @width  = width
    @height = height
  end

  def to_code(options = {})
    break_bytes = options.has_key?(:break_bytes) ? options[:break_bytes] : 8

    output = ''
    (0..@bytes.length-1).each do |index|
      output << "0x#{'%02x' % @bytes[index]}"
      output << ', ' if index < @bytes.length - 1
      output << "\n" if index % break_bytes == 0 && index > 0
    end

    output
  end

  def write_to_file(file, options = {})
    File.open(file, 'w') { |f| f.write to_code(options) }
  end

  def eql?(other)
    other.kind_of?(self.class) && other.pixels == self.pixels &&
        other.width == self.width && other.height == self.height
  end

  alias :== :eql?

end

class Tile

  def to_prog_mem
    ProgMem.from_pixels pixels, width, height
  end

end