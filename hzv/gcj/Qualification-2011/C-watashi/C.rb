1.upto(gets.to_i) do |ri|
	gets
	a = gets.split.map(&:to_i)
	if a.inject(&:^) == 0 then
		b = a.inject(:+) - a.min
	else
		b = :NO
	end
	puts "Case ##{ri}: #{b}"
end
