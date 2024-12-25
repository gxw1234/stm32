import os
import subprocess
import sys

def create_jlink_script(hex_file):
    """创建临时的 JLink 命令脚本"""
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
    """使用 JLink 烧录 hex 文件"""
    # 获取构建目录的绝对路径
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "build")
    hex_file = os.path.join(build_dir, "gxw_build_product.hex")
    
    # 检查 hex 文件是否存在
    if not os.path.exists(hex_file):
        print(f"Error: Hex file not found: {hex_file}")
        return False
    
    # 创建 JLink 命令脚本
    create_jlink_script(hex_file)
    
    # JLink 命令
    jlink_cmd = [
        "JLink.exe",  # 确保 JLink.exe 在系统路径中
        "-device", "STM32H750VB",  # 根据你的芯片型号修改
        "-if", "SWD",
        "-speed", "4000",
        "-autoconnect", "1",
        "-CommanderScript", "flash.jlink"
    ]
    
    try:
        # 执行 JLink 命令
        process = subprocess.Popen(jlink_cmd, 
                                 stdout=subprocess.PIPE, 
                                 stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        
        # 打印输出
        print(stdout.decode())
        if stderr:
            print("Errors:", stderr.decode())
            
        # 检查返回码
        if process.returncode == 0:
            print("Programming completed successfully!")
            return True
        else:
            print(f"Programming failed with return code: {process.returncode}")
            return False
            
    except Exception as e:
        print(f"Error occurred: {str(e)}")
        return False
    finally:
        # 清理临时文件
        if os.path.exists("flash.jlink"):
            os.remove("flash.jlink")

if __name__ == "__main__":
    print("Starting programming process...")
    flash_hex()
