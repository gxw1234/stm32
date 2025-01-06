import os
import subprocess
import sys

def create_jlink_script(hex_file):
    
    script_content = f"""si 1
speed 4000
r
h
loadfile {hex_file}
r
g
exit"""
    
    with open("flash.jlink", "w") as f:
        f.write(script_content)

def flash_hex():
   
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "build")
    hex_file = os.path.join(build_dir, "gxw_build_product.hex")
    
   
    if not os.path.exists(hex_file):
        print(f"Error: Hex file not found: {hex_file}")
        return False
    
   
    create_jlink_script(hex_file)
    
    
    jlink_cmd = [
         r"C:\Program Files\SEGGER\JLink_V798i\JLink.exe",  
        "-device", "STM32H750VB", 
        "-if", "SWD",
        "-speed", "4000",
        "-autoconnect", "1",
        "-CommanderScript", "flash.jlink"
    ]
    try:
       
        process = subprocess.Popen(jlink_cmd, 
                                 stdout=subprocess.PIPE, 
                                 stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        
      
        if stderr:
            print("Errors:", stderr.decode())
            
        
        if process.returncode == 0:
            # print("Programming completed successfully!")
            return True
        else:
            # print(f"Programming failed with return code: {process.returncode}")
            return False
            
    except Exception as e:
        print(f"Error occurred: {str(e)}")
        return False
    finally:
       
        if os.path.exists("flash.jlink"):
            os.remove("flash.jlink")

if __name__ == "__main__":
    print("Starting programming process...")
    flash_hex()
