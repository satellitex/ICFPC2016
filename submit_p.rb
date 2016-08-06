require './setting.rb'
N=3000

N.times{ |i|
  output_file = SUBMIT_DIR + i.to_s + '.txt'
  result_file = RESULT_DIR + i.to_s + '.txt'
  if File.exist?( result_file )
    next
  end
  if File.exist?( output_file )
    score = `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' -F 'problem_id=#{i}' -F 'solution_spec=@#{output_file}' 'http://2016sv.icfpcontest.org/api/solution/submit'`
    p output_file
    p score
    File.open( result_file, "w" ) do |file|
      file.puts( score )
    end
    sleep(2)
  end
}
