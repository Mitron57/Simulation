#ifndef LIFESYSTEM_H
#define LIFESYSTEM_H
namespace Solution {
    struct LifeSystem final : System {
        LifeSystem() = default;

        void onAwake() override;
        bool onUpdate() override;

    private:
        static bool calculateChance(std::double_t chance, std::double_t coeff = 1);
        static void generateGrass();

        template <typename T>
        static void bornChild(std::int32_t posX, std::int32_t posY);

        static std::tuple<bool, std::int32_t, std::int32_t> isNear(
            char object, const std::shared_ptr<Position>& position
        );
    };
}  // namespace Solution
#endif  // LIFESYSTEM_H
