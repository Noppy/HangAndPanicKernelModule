/*
 * Hang&Panic module for linux
 *
 * Copyright (c) 2018 n.fujita
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/* Module information */
MODULE_DESCRIPTION( "hang and panic" );
MODULE_LICENSE( "MIT" );
#define MODULE_NAME "hang_panic module"
#define PROC_NAME   "hang_panic"

static char message[64] = {0};

/* main function */
void test_panic(void)
{
    printk( KERN_INFO "panic test\n");
    panic("Panic via Panic&Hung test module");
}

void test_hang_hard(void)
{
    printk( KERN_INFO "Hang test(disable local irq and preempt)\n");

    local_irq_disable();
    preempt_disable();

    for(;;){}

    preempt_enable();
    local_irq_enable();

}

void test_hang_soft(void)
{
    printk( KERN_INFO "Hang test(disable only local irq)\n");

    local_irq_disable();

    for(;;){}

    local_irq_enable();

}


/* proc functions */
static int panic_hang_show(struct seq_file *m, void *v)
{
    seq_printf(m, "<<Hang&Panic module>>\n");
    seq_printf(m, "\'echo c > /proc/%s\' >>> panic\n", PROC_NAME);
    seq_printf(m, "\'echo h > /proc/%s\' >>> hang(disable local irq and preempt)\n", PROC_NAME);
    seq_printf(m, "\'echo H > /proc/%s\' >>> hang(disable only local irq)\n", PROC_NAME);
    return 0;
}

static ssize_t panic_hang_write(struct file *filp, const char __user *buff, size_t user_len, loff_t *offset )
{
    size_t len = 0;

    if( 0 != *offset){
        return -EINVAL;
    }
    memset(message, 0, sizeof(message));

    len = user_len > sizeof(message) ? sizeof(message) : user_len;

    memcpy( message, buff, len );
    message[len-1] = '\n';
    printk(KERN_INFO "%s\n", message);

    /* check message */
    switch( message[0] ){
      case 'c':
        test_panic();
        break;

      case 'h':
        test_hang_hard();
        break;

      case 'H':
        test_hang_soft();
        break;

      default:
        break;
    }

    return user_len;
}

static int panic_hang_open(struct inode *inode, struct file *file)
{
    return single_open(file, panic_hang_show, NULL);
}

static const struct file_operations panic_hang_proc_fops = {
    .owner   = THIS_MODULE,
    .open    = panic_hang_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
    .write   = panic_hang_write,
};


/* module initialize & remove */
static int __init hang_and_panic_init( void )
{
    printk( KERN_INFO "Install panic&hang test module\n");

    /* add proc */    
    proc_create(PROC_NAME, S_IRUSR|S_IWUSR, NULL, &panic_hang_proc_fops);

    return 0;
}

static void __exit hang_and_panic_exit( void )
{
    printk( KERN_INFO "Uninstall panic&hang test module\n");
    remove_proc_entry(PROC_NAME, NULL);

}

module_init( hang_and_panic_init );
module_exit( hang_and_panic_exit );

