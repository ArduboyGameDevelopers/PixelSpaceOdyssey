require 'zip'

module Common

  def fail_script(message)
    raise "Build failed! #{message}"
  end

  def fail_script_unless(condition, message)
    fail_script message unless condition
  end

  def fail_script_unless_file_exists(path)
    fail_script_unless path != nil && (File.directory?(path) || File.exists?(path)), "File doesn't exist: '#{path}'"
  end

  def not_nil(something)
    fail_script_unless something != nil, 'Not nil expected'
    something
  end

  def resolve_path(path)
    fail_script_unless_file_exists path
    return path
  end

  def exec_shell(command, error_message, options = {})
    puts "Running command: #{command}" unless options[:silent] == true
    result = `#{command}`
    if options[:dont_fail_on_error] == true
      puts error_message unless $?.success?
    else
      fail_script_unless($?.success?, "#{error_message}\nShell failed: #{command}\n#{result}")
    end

    return result
  end

  def zip_dir(path, out_file = nil)

    fail_script_unless_file_exists path
    out_file = "#{File.basename(path)}.zip" if out_file.nil?

    path.sub!(%r[/$],'')

    FileUtils.rm out_file, :force => true
    basedir = File.basename(out_file, '.zip')

    Zip::File.open(out_file, Zip::File::CREATE) do |zip_file|
      Dir["#{path}/**/**"].each do |file|
        dest = "#{basedir}/#{file.sub("#{path}/", '')}"
        zip_file.add(dest, file)
      end
    end

    return File.expand_path out_file
  end

end
