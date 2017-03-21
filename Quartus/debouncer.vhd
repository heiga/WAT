library ieee;
use ieee.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity debouncer is
	port 
	(
	clk 	: in std_logic;
	input : in std_logic;
	tonano : out std_logic);
end debouncer;

architecture debounce of debouncer is
	signal x: std_logic_vector(0 to 27) := x"0000000";
	signal output : std_logic := '0';
	signal activated : std_logic := '0';
	
	
	begin 
	tonano <= output;	
	process (clk)
	
	begin
		
		if(rising_edge(clk)) then
		if(activated = '0') then
			if(input = '1') then
				activated <= '1';
				output <= '1';
			end if;
		
				
		--if((rising_edge(clk)) and (activated = '1')) then
		else if(activated ='1') then
			--if(rising_edge(clk)) then
			-- start counter
				if x = x"2FAF080" then
					x <= x"0000000";
					activated <= '0';
					if(input = '0') then
						output <= '0';
					end if;		
				else
				x <= x + '1';
				end if;
			end if;
		end if;	
	end if;	
	end process;	
end debounce;
