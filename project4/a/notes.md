# Problems encountered

## Could not log in to `root` using `ssh`

After flashing the beaglebone, I tried to access it through `ssh`. However, the command `ssh root@192.168.6.2` prompted me for a password so I followed what the internet said, which was simply pressing enter since there is no password initially.

```
$ ssh root@192.168.6.2
Permission denied, please try again
root@192.168.6.2's password:
Permission denied, please try again.
root@192.168.6.2's password:
root@192.168.6.2: Permission denied (publickey,password).
```

Then I tried to used a work-around, which was logining in to `debian` instead of `root` and changing `root`'s password.

```bash
sudo -s
passwd root
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
```

But even the I still couldn't login to root through ssh.

I also tried `$ sudo login root` and still got the same result.

I later found out that the reason I could not login to `root` through `ssh` was because the beaglebone's `sshd_config` did not allow clients to login to `root` through `ssh` using password. Therefore, even if I had the right password, it would still say `Permission denied`. Therefore, a solution to this is to make changes to the config for `sshd_config` in `/etc/ssh/sshd_config` which will allow root login with password. Specifically, we need to change from

`PermitRootLogin without-password` 

to 

`PermitRootLogin yes`. 

Then we need to restart the ssh server.

```bash
$ /etc/init.d/ssh restart
[ ok ] Restarting ssh (via systemctl): ssh.service.
```

### Source:

* <https://linuxconfig.org/enable-ssh-root-login-on-debian-linux-server>
* <https://v37e00e.blogspot.com/2014/04/beaglebone-black-bbb-debian-root.html>

## Did not flash the Beaglebone correctly

After bringing the Beaglebone to work today, I was no longer able to log in to the Beagle bone through `ssh` to both `root` and `debian`.

```bash
$ ssh root@192.168.6.2
ssh: connect to host 192.168.6.2 port 22: Network is unreachable

$ ssh debian@192.168.6.2
ssh: connect to host 192.168.6.2 port 22: Network is unreachable
```

I then tried `sudo ssh` and succeeded in login in to `root`. However when I checked the for the debian version, it was **8.6**, not **9.5** even though I already flashed the beaglebone to **9.5** yesterday. I also noticed that the user I created for CS 35L was also there, which was named `son`

```bash
$ sudo ssh root@192.168.7.2
# cat /etc/debian_version
8.6
# ls /home
debian		son
```

I then tried `sudo ssh` to login to `debian` and check for the debian version, which turned out to also be **8.6**. I also noticed that in the home folder of `debian` there was a folder `ducanh` which was created when I lent the Beaglebone to Duc Anh.

```bash
$ sudo ssh debian@192.168.7.2
$ cat /etc/debian_version
8.6
$ pwd
/home/debian
$ ls
bin		ducanh
```

I relized that I had not flashed the Beaglebone correctly. Specifically I did not flash the Beaglebone's on-board **eMMC**.

### Thought process

You can boot up your Beaglebone with either a MICROSD or the Beaglebone's 4GB on-board eMMC. Therefore when I thought I flashed the entire board (i.e.factory reset). I actually flashed Debian 9.5 on the MICROSD and by holding the `USER` button on the Beaglebone, I actually booted the Beaglebone with Debian 9.5 on from MICROSD instead of booting from the on-board memory (which was still Debian 8.6 with all the old data.

Part of this was also because I downloaded the wrong image for the flashing. I downloaded Debian 9.5 from the **Older Debian images** on the website with the description "BeagleBone compatibles via microSD card (without flashing the eMMC)". What I should have done was downloading images with the description either "Buster IoT (without graphical desktop) for BeagleBone on-board eMMC flashing via microSD card" or "eMMC flasher".

#### Source:

* <http://derekmolloy.ie/write-a-new-image-to-the-beaglebone-black/>
* <https://stackoverflow.com/questions/31725206/unable-to-flash-emmc-from-sd-card-beaglebone-black>
* <https://www.datalight.com/solutions/technologies/emmc/what-is-emmc>

## Did not know how to change the name of the Beaglebone

* When you visit a website, you type that website's URL into your browser. That URL is made up of regular words that you can easily understand, like "google.com". Behind the scenes, a DNS server associates that name with a server's IP address. This system exists because remebering IP addresses is awful, and no one would do it.
* Hosts names work the same way, only on a local network
* When you give your computer a hostname, it's a regular word that you understand and can easily type in to identify that computer on your network.
* Just like a URL, you can substitute that hostname where you would regularly use an IP address. That include browsing local web pages and using SSH.

### See your host name

```
$ hostname
beaglebone
```

### Changing your host name

The file `/etc/hostname` is where your hostname is stored. First you need to change the name that is currently there to your desired name.

```
$ sudo nano /etc/hostname
```

The BeagleBone won't recognize this new name you have just modified, so you need to edit the `/etc/hosts` file and change where it reads `beaglebone` to your new host's name.

```
$ sudo nano /etc/hosts
```

### Not able to `ping google.com`

I tried every possible way I could to solve this problem since it kept saying `Temporary failure in name resolution`

Turns out I did not connect the **antennas** which are used to connect to wifi.

### Not able to `ssh` to the Beaglebone via wifi

I was confused to how I could establish a connection between my laptop and the Beaglebone over wifi. Turns out when the Beaglebone was connected to the same wifi as the laptop, it was given an IP address. You can `ssh` to the Beaglebone from your laptop by `ssh` to the IP address of the Beaglebone on the network. To find your IP address, simply run `ifconfig` on the Beaglebone and look for the numbers right next to `inet` in the `wlan0` section. If the IP address of the Beaglebone is `192.168.1.11`, you would do.

```
ssh root@192.168.1.11
```

Note that the BeagleBone wifi where you see on your laptop scanner is for a different purpose, not `ssh` from your machine to the Beaglebone. They are two different kinds of wifi.