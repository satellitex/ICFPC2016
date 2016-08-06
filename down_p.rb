require 'json'
require 'net/http'
require 'uri'

json_file_path = './out'


json_data = open(json_file_path) do | io |
  JSON.load(io)
end



problems = json_data['problems']
problems.each{ |problem|
  p_id = problem['problem_id']
  p_hash = problem['problem_spec_hash']

  fname = "./inputs/"+p_id.to_s+".txt"
  
  if File.exist?( fname )
    next
  end
  
  response = `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/blob/#{p_hash}'`
    

  p fname
  p response
  File.open(fname,"w") do | file |
    file.puts( response )
  end
  sleep(0.5)
}

