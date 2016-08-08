
cnt = 0
N=10000
N.times{ |i|  
  fname = "./results/" + (i+1).to_s + ".txt"
  if File.exist?( fname )
    
    File.open( fname, "r" ) do | file |
      flag = false
      file.each_line{ |line|
        if line.include?("false") or line.include?("0.0,")         
          flag = true
        end
      }
      if flag
        p fname
        cnt += 1
        #`rm #{fname}`
      end
    end
  end
}
puts cnt
