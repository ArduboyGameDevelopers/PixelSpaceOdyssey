class ProgMem

  attr_reader :height, :width, :bytes

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