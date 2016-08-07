require 'json'

cnt = 0
while true
  
  puts cnt.to_s + " times"
  cnt += 1
  
  snap = `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/snapshot/list'`
  p snap
  json_data = JSON.load( snap )
  
  sleep(2)
  
  snap = json_data["snapshots"].last  
  shash = snap["snapshot_hash"]

  p shash
  
  `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/blob/#{shash}' > b.json`

  sleep(2)

  puts "exec down_p.rb"
  `ruby down_p.rb`

  sleep(2)
  
  puts "exec rmfile.rb"
  `ruby rmfile.rb`

  puts "exec output_p.rb"
  `ruby output_p.rb`

  puts "exec submit_p.rb"
  `ruby submit_p.rb`

  sleep(2)
  
end
