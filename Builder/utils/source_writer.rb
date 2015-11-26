class SourceWriter

  def initialize(tab = '  ')
    @source = ''
    @newline = true
    @tab = tab
    @tabs = ''
  end

  def print(line)
    if @newline
      @source << @tabs
      @newline = false
    end

    @source << line
  end

  def println(line = '')
    print "#{line}\n"
    @newline = true
  end

  def block_open()
    println '{'
    inc_tab
  end

  def block_close(separator = '')
    dec_tab
    println "}#{separator}"
  end

  def write_to_file(path)
    File.open(path, 'w') { |file| file.write @source }
  end

  def to_s
    @source
  end

  private
  def inc_tab()
    @tabs << @tab
  end

  private
  def dec_tab()
    raise 'Unballance tabs' if @tabs.length == 0
    @tabs = @tabs.length > @tab.length ? @tabs[@tab.length, -1] : ''
  end

end