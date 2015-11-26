module Utils
  def self.to_identifier(name)
    name = name.downcase
    name = name.gsub '-', '_'
    name = name.gsub ' ', '_'
    name
  end
end