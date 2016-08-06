require './setting.rb'
N=3000
N.times { |i|
  input_file = INPUT_DIR + i.to_s + '.txt'
  output_file = SUBMIT_DIR + i.to_s + '.txt'
  if File.exist?( input_file )
    if File.exist?( output_file )
      next
    end
    puts i
    `#{EXE_FILE} < #{input_file} > #{output_file}`    
  end
}

