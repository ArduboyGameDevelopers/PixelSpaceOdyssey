require 'chunky_png'
require_relative 'prog_mem'

module ChunkyPNG

  COLOR_BLACK = 0x000000ff
  COLOR_TRANSPARENT = 0xffffff00

  class Canvas
    def sub_image(x, y, w, h)
      sub_pixels = []
      (y..y+h-1).each do |y|
        (x..x+w-1).each do |x|
          pixel = pixels[y * width + x]
          alpha = Color.a pixel
          sub_pixels.push (alpha > 0 && !is_white(pixel) ? COLOR_BLACK : COLOR_TRANSPARENT)
        end
      end
      Image.new w, h, sub_pixels
    end

    def to_prog_mem
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
            byte |= (1 << (bit_height)) if Color.a(pixel) > 128
          end
          bytes << byte
        end
      end

      ProgMem.new bytes, width, height
    end

    def is_transparent
      pixels.each { |pixel|
        return false if Color.a(pixel) > 0
      }
      true
    end

    private
    def is_white(color)
      (color >> 8) == 0xffffff
    end

  end
end