require 'chunky_png'
require_relative 'rect'
require_relative 'frame'

module ImageUtils

  def self.make_frame(name, png, transparent = 0xffffffff)

    pixels = png.pixels
    width = png.width
    height = png.height

    top = 0
    left = 0
    bottom = height-1
    right = width-1

    # top
    (0..height-1).each { |y|
      done = false
      (0..width-1).each { |x|
        pixel = pixels[width * y + x]
        if pixel != transparent
          top = y
          done = true
          break
        end
      }
      break if done
    }

    # left
    (0..width-1).each { |x|
      done = false
      (0..height-1).each { |y|
        pixel = pixels[width * y + x]
        if pixel != transparent
          left = x
          done = true
          break
        end
      }
      break if done
    }

    # bottom
    (0..height-1).reverse_each { |y|
      done = false
      (0..width-1).each { |x|
        pixel = pixels[width * y + x]
        if pixel != transparent
          bottom = y
          done = true
          break
        end
      }
      break if done
    }

    # right
    (0..width-1).reverse_each { |x|
      done = false
      (0..height-1).each { |y|
        pixel = pixels[width * y + x]
        if pixel != transparent
          right = x
          done = true
          break
        end
      }
      break if done
    }

    frame_width = right - left + 1
    frame_height = bottom - top + 1
    frame_bits  = Array.new # only store 0 or 1

    (top..bottom).each { |y|
      (left..right).each { |x|
        pixel = pixels[y * width + x]
        frame_bits << (pixel == transparent ? 0 : 1)
      }
    }

    data = convert_to_display_format frame_bits, frame_width, frame_height

    rect = Rect.new left, top, frame_width, frame_height
    return Frame.new name, data, rect

  end

  def self.convert_to_display_format(frame_bits, frame_width, frame_height)
    bits_last_page = frame_height % 8
    bytes_high = frame_height / 8
    bytes_high += 1 if bits_last_page > 0

    data = []

    (0..bytes_high - 1).each do |y_page|
      (0..frame_width - 1).each do |x|
        # how many bits does this line hold
        bits = 8

        # if we've reached the bottom there are fewer bits to load
        bits = bits_last_page if bytes_high - 1 == y_page and bits_last_page > 0
        byte = 0
        (0..bits-1).each do |bit_height|
          pixel_index = (y_page * 8 + bit_height) * frame_width + x
          pixel_bit = frame_bits[pixel_index]
          byte |= (1 << (bit_height)) if pixel_bit == 1
        end
        data << byte
      end
    end

    data
  end

end