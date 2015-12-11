require_relative 'common'

include Common

$project_name     = 'PixelSpaceOdyssey'
$project_config   = 'Debug'

$dir_builder      = File.expand_path '.'
$dir_repo         = File.expand_path '..'
$dir_project      = resolve_path "#{$dir_repo}/#{$project_name}"

$dir_out          = "#{$dir_builder}/out"

$dir_emu          = resolve_path "#{$dir_repo}/Emulator"
$dir_emu_build    = "#{$dir_emu}/build"
$file_emu_project = resolve_path "#{$dir_emu}/Emulator.pro"

def extract_project_version(dir_project)
  file_plist = resolve_path "#{dir_project}/Version.h"
  source = File.read file_plist
  source =~ /#define\s+PROJECT_VERSION\s+"(\d+\.\d+\.\d+)"/

  not_nil $1
end

project_version = extract_project_version $dir_emu

puts "Project version: #{project_version}"

# clean up
FileUtils.rm_rf $dir_emu

# build make project
Dir.chdir $dir_emu do
  exec_shell %(qmake "#{$file_emu_project}" -r -spec win32-g++ "CONFIG+=#{$project_config.downcase}), "Can't run qmake"
end

# run make project
dir_makefile = "#{$dir_emu_build}/#{$project_config}"
file_makefile = "Makefile.#{$project_config}"

Dir.chdir dir_makefile do
  exec_shell %(mingw32-make -f "#{file_makefile}"), "Can't run make"
end