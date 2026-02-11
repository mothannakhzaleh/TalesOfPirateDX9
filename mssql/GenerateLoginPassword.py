import hashlib
import binascii

def blake2s_hash(password):
    # Create a BLAKE2s hash object
    hasher = hashlib.blake2s()

    # Update the hasher with the password (string should be encoded to bytes)
    hasher.update(password.encode('utf-8'))

    # Get the digest (hash result)
    digest = hasher.digest()

    # Convert the digest to a hexadecimal string
    hexencoded = binascii.hexlify(digest).decode('utf-8')

    return hexencoded

password = "admin"
hexencoded = blake2s_hash(password)
print(hexencoded)