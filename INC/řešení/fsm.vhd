-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S3B, S4B, S5B, S6B, S7B, S8B, S9B, S10B, S11, PRINT_SPATNE, PRINT_SPRAVNE, CHYBA, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= S1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S1 =>
      next_state <= S1;
      if (KEY(1) = '1') then
         next_state <= S2; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S2 =>
      next_state <= S2;
      if (KEY(1) = '1') then
         next_state <= S3; 
      elsif (KEY(6) = '1') then
         next_state <= S3B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S3 =>
      next_state <= S3;
      if (KEY(6) = '1') then
         next_state <= S4; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S3B =>
      next_state <= S3B;
      if (KEY(3) = '1') then
         next_state <= S4B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S4 =>
      next_state <= S4;
      if (KEY(6) = '1') then
         next_state <= S5; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S4B =>
      next_state <= S4B;
      if (KEY(2) = '1') then
         next_state <= S5B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S5 =>
      next_state <= S5;
      if (KEY(1) = '1') then
         next_state <= S6; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S5B =>
      next_state <= S5B;
      if (KEY(6) = '1') then
         next_state <= S6B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S6 =>
      next_state <= S6;
      if (KEY(5) = '1') then
         next_state <= S7; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S6B =>
      next_state <= S6B;
      if (KEY(1) = '1') then
         next_state <= S7B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S7 =>
      next_state <= S7;
      if (KEY(0) = '1') then
         next_state <= S8; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S7B =>
      next_state <= S7B;
      if (KEY(0) = '1') then
         next_state <= S8B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S8 =>
      next_state <= S8;
      if (KEY(0) = '1') then
         next_state <= S9; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S8B =>
      next_state <= S8B;
      if (KEY(0) = '1') then
         next_state <= S9B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S9 =>
      next_state <= S9;
      if (KEY(6) = '1') then
         next_state <= S10; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S9B =>
      next_state <= S9B;
      if (KEY(9) = '1') then
         next_state <= S10B; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S10 =>
      next_state <= S10;
      if (KEY(7) = '1') then
         next_state <= S11; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S10B =>
      next_state <= S10B;
      if (KEY(4) = '1') then
         next_state <= S11; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      elsif (KEY(15) = '1') then
        next_state <=PRINT_SPATNE;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when S11 =>
      next_state <= S11;
      if (KEY(15) = '1') then
         next_state <= PRINT_SPRAVNE; 
      elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;        
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CHYBA =>
      next_state <= CHYBA;
      if (KEY(15) = '1') then
         next_state <= PRINT_SPATNE; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SPATNE =>
      next_state <= PRINT_SPATNE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SPRAVNE =>
      next_state <= PRINT_SPRAVNE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= S1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= S1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
 -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SPRAVNE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SPATNE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '0';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   if (KEY(14 downto 0) /= "000000000000000") then
      FSM_LCD_WR     <= '1';
   elsif(KEY(15) = '1') then
      FSM_LCD_CLR    <= '1';
   end if;  
   end case;
end process output_logic;

end architecture behavioral;


