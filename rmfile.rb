
cnt = 0
N=2335
N.times{ |i|  
  fname = "./inputs/" + (i+1).to_s + ".txt"
  if File.exist?( fname )
    
    File.open( fname, "r" ) do | file |
      flag = false
      file.each_line{ |line|
        if line.include?("Too")
          flag = true
        end
      }
      if flag
        cnt += 1
        `rm #{fname}`
      end
    end
  end
}
puts cnt
