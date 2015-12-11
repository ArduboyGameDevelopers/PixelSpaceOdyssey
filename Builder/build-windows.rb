require_relative 'dropbox_deploy'
require_relative 'dropbox_deploy_credentials'
require_relative 'common'

include Common

$project_name     = 'PixelSpaceOdyssey'
$project_config   = 'Release'

$dir_builder      = File.expand_path '.'
$dir_repo         = File.expand_path '..'
$dir_project      = resolve_path "#{$dir_repo}/#{$project_name}"

$dir_out          = "#{$dir_builder}/out"
$dir_out_builds   = "#{$dir_out}/builds"

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
dir_build = "#{$dir_emu_build}/#{$project_config}"

FileUtils.rm_rf dir_build
FileUtils.mkpath dir_build

# build make project
Dir.chdir dir_build do
  exec_shell %(qmake "#{$file_emu_project}" -r -spec win32-g++ "CONFIG+=#{$project_config.downcase}), "Can't run qmake"
end

# run make project
file_makefile = "Makefile.#{$project_config}"

Dir.chdir dir_build do
  exec_shell %(mingw32-make -f "#{file_makefile}"), "Can't run make"
end

# deploy windows
file_app = resolve_path "#{dir_build}/#{$project_config.downcase}/Emulator.exe"
dir_deploy = "#{$dir_out}/deploy"
FileUtils.rm_rf dir_deploy
FileUtils.mkpath dir_deploy

FileUtils.cp file_app, "#{dir_deploy}/"

Dir.chdir dir_deploy do
  cmd = ''
  cmd << "windeployqt"
  cmd << " --#{$project_config.downcase}"
  cmd << " --no-translations"
  cmd << " --no-quick-import"
  cmd << " --no-system-d3d-compiler"
  cmd << " --no-angle"
  cmd << " Emulator.exe"
  exec_shell cmd, "Can't deploy windows"

  trash_files = [
      'iconengines',
      'imageformats',
      'opengl32sw.dll',
      'Qt5Svg.dll'
  ]

  trash_files.each do |file|
    if File.directory? file
      FileUtils.rm_rf file
    else
      FileUtils.rm_f file
    end
  end

end

# copy tiles
FileUtils.cp_r "#{$dir_emu}/Tiles", "#{dir_deploy}/"

# zip delivery

FileUtils.rm_rf $dir_out_builds
FileUtils.mkpath $dir_out_builds

file_build = "#{$dir_out_builds}/#{$project_name}-#{project_version}.zip"
zip_dir dir_deploy, file_build

dropbox = DropboxDeploy.new $dropbox_access_token
dropbox.deploy file_build