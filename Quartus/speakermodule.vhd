library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
entity speakerinterface is
	port 
	(
	clk : in std_logic;
	ena : in std_logic;
	tospeaker : out std_logic);
end speakerinterface;

architecture speak of speakerinterface is
	signal x: std_logic_vector(0 to 15) := x"0000";
	signal output : std_logic := '0';
	
	
	begin 
	tospeaker <= output;	
	process (clk, ena)
	
		begin
		if(rising_edge(clk)) then
			if x = x"61A8" then
			output <= not output;
			x <= x"0000";
			end if;
		
			if ena = '1' then
			x <= x + 1;
			end if;
		end if;		
	end process;	
end speak;
