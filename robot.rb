require 'json'

cnt = 0
while true

  puts cnt + " times"
  cnt += 1
  
  snap = `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/snapshot/list'`
  
  json_data = JSON.load( snap )
  
  sleep(2)
  
  snap = json_data["snapshots"].last
  shash = snap["snapshot_hash"]

  `curl --compressed -L -H Expect: -H 'X-API-Key: 297-509936fdbd7abfd41c69de9400d121f9' 'http://2016sv.icfpcontest.org/api/blob/#{shash}' > b.json`

  sleep(2)

  `ruby down_p.rb`

  sleep(2)

  `ruby rmfile.rb`

  `ruby output_p.rb`

  `ruby submit_p.rb`

  sleep(2)
  
end
