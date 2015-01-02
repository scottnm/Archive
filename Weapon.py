class Weapon(object):
    def fire(self):
        raise Exception("Must implement 'Fire' method")

    def reload(self):
        raise Exception("Must implement 'reload' method")


class Pistol(Weapon):
    def __init__(self):
        self.CLIP_CAPACITY = 6
        self.ammo_left = 6

    def fire(self):
        if self.ammo_left > 0:
            # return a new bullet to display
            self.ammo_left -= 1

        return None

    def reload(self):
        self.ammo_left = 6


# class Shotgun( Weapon ):

# class Crossbow( Weapon ):
