
hash = "f4b1a8567108144bae331340a57c68b85df487e0"
a = `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/blob/#{hash}'`

File.open("unko","w") do |file|
  file.puts( a )
end
