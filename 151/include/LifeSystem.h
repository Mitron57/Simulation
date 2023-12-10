#ifndef LIFESYSTEM_H
#define LIFESYSTEM_H
namespace Solution {
    struct LifeSystem final : System {
        LifeSystem() = default;

        void onAwake() override;
        bool onUpdate() override;

    private:
        static bool calculateChance(std::double_t chance, std::double_t coeff);
        static void generateGrass();

        template <typename T>
        static void bornChild(std::int32_t posX, std::int32_t posY);
    };
}
#endif //LIFESYSTEM_H
