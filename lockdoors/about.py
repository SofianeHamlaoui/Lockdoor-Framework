from lockdoors import main
from lockdoors import shrts
def show():
    shrts.clscprilo()
    print("""

    #############################################################
    #                   Lockdoor Framework                      #
    #  A Penetration Testing framework with CyberSec Resources  #
    #############################################################
    #    -- Version: v2.3 16/06/2021                            #
    #    -- Developer: Sofiane Hamlaoui                         #
    #    -- Thanks: No One                                      #
    #############################################################

                        \033[94m-[!]-Description-[!]-\033[91m
   LockDoor is a Framework aimed at helping penetration testers,
   bug bounty hunters And cyber security engineers.
   This tool is designed for Debian/Ubuntu/ArchLinux based
   distributions to create a similar and familiar distribution
   for Penetration Testing. But containing the favorite and the most used tools by
   Pentesters.
   As pentesters, most of us has his personal ' /pentest/ ' directory so this
   Framework is helping you to build a perfect one.
    """)
    shrts.oktocont()
    main.menu()
