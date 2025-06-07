import unittest
import vmmap

class TestVMMapParse(unittest.TestCase):
    def test_parse_vmmap_line(self):
        result = vmmap.parse("__AUTH                      28ab475e8-28ab47778    [    0.04     0.03     0.02     0   ] rw-/rw- SM=COW          /System/Library/PrivateFrameworks/WritingTools.framework/Versions/A/WritingTools")
        self.assertTrue(result != None)
        self.assertEqual(result.name, "__AUTH")
        self.assertEqual(result.description, "rw-/rw- SM=COW          /System/Library/PrivateFrameworks/WritingTools.framework/Versions/A/WritingTools")
        self.assertEqual(result.start_address, 0x28ab475e8)
        self.assertEqual(result.end_address, 0x28ab47778)
        self.assertEqual(result.vsize, 0.04)
        self.assertEqual(result.rsize, 0.03)
        self.assertEqual(result.dirty_size, 0.02)
        self.assertEqual(result.swap_size, 0)
        pass

    def test_parse_vmmap_line_kernel(self):
        result = vmmap.parse("page table in kernel           kernel-kernel       [  21   22   23     0] rw-/rw- SM=PRV          charged to process physical footprint")
        self.assertTrue(result != None)
        self.assertEqual(result.name, "page table in kernel")
        self.assertEqual(result.description, "rw-/rw- SM=PRV          charged to process physical footprint")
        self.assertEqual(result.start_address, 0)
        self.assertEqual(result.end_address, 0)
        self.assertEqual(result.vsize, 21)
        self.assertEqual(result.rsize, 22)
        self.assertEqual(result.dirty_size, 23)
        self.assertEqual(result.swap_size, 0)
        pass
