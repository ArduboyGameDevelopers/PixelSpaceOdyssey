require 'dropbox_sdk'

class DropboxDeploy

  def initialize(access_token)
    @client = DropboxClient.new access_token
  end

  def deploy(path)
    file = open path
    filename = File.basename path
    response = @client.put_file "/#{filename}", file
    puts "Uploaded: #{response.inspect}"
  end

end