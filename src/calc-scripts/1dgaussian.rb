#!/usr/bin/ruby

nsamples = 10;

i = -nsamples;
g = []
g_sum = 0.0
sigma = 4.0;
while(i <= nsamples) do
  temp = Math.exp(-i.to_f*i.to_f/2.0/sigma/sigma)
  g_sum += temp
  g << temp 
  i += 1
end
g.map! { |i| i/g_sum/2.0 }

i = -nsamples.to_f;
g.each do |gvalue|
  puts "color += texture2D(tex, vec2(texcoord.x + #{i}*pixelsize, texcoord.y)) * #{gvalue};"
  i += 1;
end
puts ""

i = -nsamples.to_f;
g.each do |gvalue|
  puts "color += texture2D(tex, vec2(texcoord.x, texcoord.y + #{i}*pixelsize)) * #{gvalue};"
  i += 1;
end
